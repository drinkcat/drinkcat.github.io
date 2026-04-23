# Umami Analytics Data Backup

Regularly fetch all Umami analytics data via REST API and save locally.

## Setup

- **Website ID:** `f40f048f-9f8c-4880-a138-63ca514d4356`
- **Cloud base URL:** `https://api.umami.is/v1`
- **Auth:** `x-umami-api-key: <key>` header (generate in Umami profile → Settings → API keys)
- **Rate limit:** 50 calls per 15 seconds

## Key Endpoints

All require `startAt` and `endAt` as millisecond timestamps.

| Endpoint | Data |
|---|---|
| `GET /websites/:id/daterange` | Available data range |
| `GET /websites/:id/stats` | Summary stats (pageviews, visitors, bounces, totaltime) |
| `GET /websites/:id/pageviews` | Pageview + session time series (`unit`: minute/hour/day/month/year) |
| `GET /websites/:id/metrics` | Aggregated by `type`: path, referrer, browser, os, device, country, region, city, language, title, hostname |
| `GET /websites/:id/metrics/expanded` | Expanded metrics with pageviews, visitors, bounces per entry |
| `GET /websites/:id/events` | Individual event records (paginated: `page`, `pageSize`) |
| `GET /websites/:id/events/series` | Event time series |
| `GET /websites/:id/events/stats` | Aggregated event counts |
| `GET /websites/:id/sessions` | Individual session records (paginated) |
| `GET /websites/:id/sessions/stats` | Session summary stats |
| `GET /websites/:id/sessions/weekly` | Sessions by hour-of-weekday (7×24 matrix) |
| `GET /websites/:id/event-data/events` | Event names with property counts |
| `GET /websites/:id/event-data/fields` | Event property names + value counts |
| `GET /websites/:id/active` | Active visitors right now |

## Backup Script Strategy

1. Call `/daterange` to get full available data range
2. Fetch summary: `/stats`, `/pageviews?unit=day`, `/metrics?type=<each type>`
3. Paginate through `/events` and `/sessions` for raw records
4. Save as timestamped JSON files
5. Run on a cron (e.g. daily), using last fetch time as `startAt` for incremental updates

## Example

```bash
START=$(date -d '30 days ago' +%s)000
END=$(date +%s)000
WEBSITE_ID="f40f048f-9f8c-4880-a138-63ca514d4356"

curl -s "https://api.umami.is/v1/websites/$WEBSITE_ID/pageviews?startAt=$START&endAt=$END&unit=day" \
  -H "x-umami-api-key: YOUR_API_KEY"
```
